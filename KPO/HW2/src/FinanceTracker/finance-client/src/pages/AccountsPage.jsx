import React from "react";
import { useQuery, useMutation } from "@tanstack/react-query";
import { api } from "../api/api";
import { Alert, Grid, Paper, Stack, TextField, Button, Typography } from "@mui/material";
import { useCurrentUser } from "../state/UserContext";
import AccountCard from "../components/AccountCard";

export default function AccountsPage() {
    const { user } = useCurrentUser();
    const enabled = !!user?.id;

    const q = useQuery({
        queryKey: ["accounts", user?.id],
        queryFn: async () => (await api.get(`/accounts/${user.id}`)).data,
        enabled,
    });

    const create = useMutation({
        mutationFn: (payload) => api.post("/accounts", payload),
        onSuccess: () => q.refetch(),
    });

    if (!user) return <Alert severity="info">Сначала выберите пользователя на главной странице.</Alert>;

    return (
        <Stack spacing={3}>
            <Typography variant="h5">💳 Счета ({user.name})</Typography>

            <Paper sx={{ p: 2 }}>
                <AddAccountForm onAdd={(name) => create.mutate({ userId: user.id, name, currency: "RUB", balance: 0 })} />
            </Paper>

            <Grid container spacing={2}>
                {q.data?.map((a) => (
                    <Grid item xs={12} md={6} lg={4} key={a.id}>
                        <AccountCard name={a.name} currency={a.currency} balance={a.balance} />
                    </Grid>
                ))}
            </Grid>
        </Stack>
    );
}

function AddAccountForm({ onAdd }) {
    const [name, setName] = React.useState("");

    return (
        <Stack direction="row" spacing={2} component="form"
               onSubmit={(e) => { e.preventDefault(); if (name) onAdd(name); setName(""); }}
        >
            <TextField label="Название счёта" value={name} onChange={(e) => setName(e.target.value)} fullWidth />
            <Button type="submit" variant="contained">Создать</Button>
        </Stack>
    );
}