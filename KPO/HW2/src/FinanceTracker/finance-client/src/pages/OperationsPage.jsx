import React from "react";
import { useQuery, useMutation } from "@tanstack/react-query";
import { api } from "../api/api";
import {
    Alert, Box, Button, Grid, MenuItem, Paper, Stack, TextField, Typography,
} from "@mui/material";
import { useCurrentUser } from "../state/UserContext";
import OperationTable from "../components/OperationTable";

export default function OperationsPage() {
    const { user } = useCurrentUser();
    const enabled = !!user?.id;

    const qOps = useQuery({
        queryKey: ["operations", user?.id],
        queryFn: async () => (await api.get(`/operations/${user.id}`)).data,
        enabled,
    });

    const qAcc = useQuery({
        queryKey: ["accounts", user?.id],
        queryFn: async () => (await api.get(`/accounts/${user.id}`)).data,
        enabled,
    });

    const qCat = useQuery({
        queryKey: ["categories", user?.id],
        queryFn: async () => (await api.get(`/categories/${user.id}`)).data,
        enabled,
    });

    const addOp = useMutation({
        mutationFn: (payload) => api.post("/operations", payload),
        onSuccess: () => qOps.refetch(),
    });

    if (!user) return <Alert severity="info">Выберите пользователя.</Alert>;

    return (
        <Stack spacing={3}>
            <Typography variant="h5">📋 Операции</Typography>

            <Paper sx={{ p: 2 }}>
                <NewOperationForm
                    accounts={qAcc.data ?? []}
                    categories={qCat.data ?? []}
                    onSubmit={(p) => addOp.mutate({
                        userId: user.id,
                        accountId: p.accountId,
                        categoryId: p.categoryId || null,
                        amount: p.isExpense ? -Math.abs(p.amount) : Math.abs(p.amount),
                        occurredOn: p.date, // ISO yyyy-mm-dd
                        description: p.desc || null,
                    })}
                />
            </Paper>

            <OperationTable rows={qOps.data ?? []} />
        </Stack>
    );
}

function NewOperationForm({ accounts, categories, onSubmit }) {
    const [accountId, setAccountId] = React.useState("");
    const [categoryId, setCategoryId] = React.useState("");
    const [isExpense, setIsExpense] = React.useState(true);
    const [amount, setAmount] = React.useState("");
    const [date, setDate] = React.useState(new Date().toISOString().slice(0, 10));
    const [desc, setDesc] = React.useState("");

    return (
        <Box component="form"
             onSubmit={(e) => { e.preventDefault(); if (accountId && amount) onSubmit({ accountId, categoryId, isExpense, amount: +amount, date, desc }); }}
             sx={{ display: "grid", gap: 2, gridTemplateColumns: { xs: "1fr", md: "1fr 1fr 1fr" } }}
        >
            <TextField select label="Счёт" value={accountId} onChange={(e) => setAccountId(e.target.value)} required>
                {accounts.map((a) => <MenuItem key={a.id} value={a.id}>{a.name}</MenuItem>)}
            </TextField>

            <TextField select label="Категория (необязательно)" value={categoryId} onChange={(e) => setCategoryId(e.target.value)}>
                <MenuItem value="">— без категории —</MenuItem>
                {categories.map((c) => <MenuItem key={c.id} value={c.id}>{c.name}</MenuItem>)}
            </TextField>

            <TextField select label="Тип" value={isExpense ? "expense" : "income"} onChange={(e) => setIsExpense(e.target.value === "expense")}>
                <MenuItem value="expense">Расход</MenuItem>
                <MenuItem value="income">Доход</MenuItem>
            </TextField>

            <TextField type="number" label="Сумма" value={amount} onChange={(e) => setAmount(e.target.value)} required />
            <TextField type="date" label="Дата" value={date} onChange={(e) => setDate(e.target.value)} />
            <TextField label="Описание" value={desc} onChange={(e) => setDesc(e.target.value)} />
            <Button type="submit" variant="contained">Добавить</Button>
        </Box>
    );
}