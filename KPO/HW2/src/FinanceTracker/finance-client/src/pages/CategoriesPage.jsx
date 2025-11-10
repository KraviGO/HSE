import React from "react";
import { useQuery, useMutation } from "@tanstack/react-query";
import { api } from "../api/api";
import {
    Alert, Button, Checkbox, FormControlLabel, Grid, Paper, Stack, TextField, Typography,
} from "@mui/material";
import { useCurrentUser } from "../state/UserContext";

export default function CategoriesPage() {
    const { user } = useCurrentUser();
    const enabled = !!user?.id;

    const q = useQuery({
        queryKey: ["categories", user?.id],
        queryFn: async () => (await api.get(`/categories/${user.id}`)).data,
        enabled,
    });

    const create = useMutation({
        mutationFn: (payload) => api.post("/categories", payload),
        onSuccess: () => q.refetch(),
    });

    const remove = useMutation({
        mutationFn: (id) => api.delete(`/categories/${id}`),
        onSuccess: () => q.refetch(),
    });

    if (!user) return <Alert severity="info">Выберите пользователя.</Alert>;

    return (
        <Stack spacing={3}>
            <Typography variant="h5">🏷 Категории</Typography>

            <Paper sx={{ p: 2 }}>
                <NewCategoryForm onSubmit={(p) => create.mutate({ ...p, userId: user.id })} />
            </Paper>

            <Grid container spacing={2}>
                {q.data?.map((c) => (
                    <Grid item key={c.id} xs={12} md={6} lg={4}>
                        <Paper sx={{ p: 2, display: "flex", justifyContent: "space-between", alignItems: "center" }}>
                            <div>
                                <Typography variant="subtitle1">{c.name}</Typography>
                                <Typography variant="body2" color="text.secondary">
                                    {c.isIncome ? "Доход" : "Расход"}
                                </Typography>
                            </div>
                            <Button color="error" onClick={() => remove.mutate(c.id)}>Удалить</Button>
                        </Paper>
                    </Grid>
                ))}
            </Grid>
        </Stack>
    );
}

function NewCategoryForm({ onSubmit }) {
    const [name, setName] = React.useState("");
    const [isIncome, setIsIncome] = React.useState(false);
    return (
        <Stack direction={{ xs: "column", sm: "row" }} spacing={2}
               component="form"
               onSubmit={(e) => { e.preventDefault(); if (name) onSubmit({ name, isIncome }); setName(""); setIsIncome(false); }}
        >
            <TextField label="Название" value={name} onChange={(e) => setName(e.target.value)} fullWidth />
            <FormControlLabel
                control={<Checkbox checked={isIncome} onChange={(e) => setIsIncome(e.target.checked)} />}
                label="Категория дохода"
            />
            <Button type="submit" variant="contained">Добавить</Button>
        </Stack>
    );
}